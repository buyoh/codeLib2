pub mod vec2;
pub use vec2::Vec2;

#[derive(Debug, Clone)]
pub struct RigidBody {}

#[derive(Debug, Clone)]
pub struct RigidState {
    pub position: Vec2,
    pub angle: f64,
}

#[derive(Debug, Clone)]
pub struct RigidPower {
    // 中心から相対的な位置及び向き
    pub position: Vec2,
    pub power: Vec2,
}

impl RigidPower {
    pub fn new(position: Vec2, power: Vec2) -> Self {
        RigidPower { position, power }
    }

    pub fn compose_moment(
        (power, moment): &(Vec2, f64),
        rigid_bodies: &[RigidPower],
    ) -> (Vec2, f64) {
        let mut total_power = Vec2::zero();
        let mut total_moment = 0.0;

        for rigid in rigid_bodies {
            total_power += rigid.power;
            total_moment += rigid.position.cross(rigid.power);
        }

        (total_power, total_moment)
    }
}

#[derive(Debug, Clone)]
pub struct RigidMotion {
    pub mass: f64,
    pub inertia: f64,
}

impl RigidMotion {
    pub fn new(mass: f64, inertia: f64) -> Self {
        RigidMotion { mass, inertia }
    }

    pub fn power_moment_to_accel(&self, power: Vec2,  moment: f64) -> (Vec2, f64) {
        (
            power / self.mass,
            moment / self.inertia
        )
    }
}

#[derive(Debug, Clone)]
pub struct RigidVelocity {
    // 単位はKMS
    // 位置
    pub position: Vec2,
    // 速度
    pub velocity: Vec2,
    // 質量
    // pub mass: f64,
    // 角度
    pub angle: f64,
    // 角速度
    pub angular_velocity: f64,
    // 慣性
    // pub inertia: f64,
}

impl RigidVelocity {
    pub fn new(position: Vec2, velocity: Vec2, angle: f64, angular_velocity: f64) -> Self {
        RigidVelocity {
            position,
            velocity,
            angle,
            angular_velocity,
        }
    }

    pub fn simulate(
        &self,
        time: f64,
    ) -> RigidVelocity {
        RigidVelocity {
            position: self.position + self.velocity * time,
            velocity: self.velocity,
            angle: self.angle + self.angular_velocity * time,
            angular_velocity: self.angular_velocity,
        }
    }

    // accel が時刻 0 から時刻 time_end にかけて accel1 から accel2 に変化する
    // angular_accel が時刻 0 から時刻 time_end にかけて angular_accel1 から angular_accel2 に変化する
    // 0 <= time <= time_end とする。time 時点での各ステータスを求める
    pub fn simulate_with_accel(
        &self,
        accel1: f64,
        accel2: f64,
        angular_accel1: f64,
        angular_accel2: f64,
        time: f64,
        time_end: f64,
    ) -> RigidVelocity {
        simulate(
            self,
            accel1,
            accel2,
            angular_accel1,
            angular_accel2,
            time,
            time_end,
        )
    }

    // accel が時刻 0 から時刻 time_end にかけて accel1 から accel2 に変化する
    // angular_accel が時刻 0 から時刻 time_end にかけて angular_accel1 から angular_accel2 に変化する
    // 0 <= time <= time_end とする。time 時点での各ステータスを求める
    pub fn simulate_with_accel_and_decay(
        &self,
        accel1: f64,
        accel2: f64,
        angular_accel1: f64,
        angular_accel2: f64,
        time: f64,
        time_end: f64,
        decay: f64,
        angular_decay: f64,
    ) -> RigidVelocity {
        simulate_decay(
            self,
            accel1,
            accel2,
            decay,
            angular_accel1,
            angular_accel2,
            angular_decay,
            time,
            time_end,
        )
    }
}

// TODO: 速度に依存した力の合成

// accel が時刻 0 から時刻 time_end にかけて accel1 から accel2 に変化する
// angular_accel が時刻 0 から時刻 time_end にかけて angular_accel1 から angular_accel2 に変化する
// 0 <= time <= time_end とする。time 時点での各ステータスを求める
fn simulate(
    rigid_body: &RigidVelocity,
    accel1: f64,
    accel2: f64,
    angular_accel1: f64,
    angular_accel2: f64,
    time: f64,
    time_end: f64,
) -> RigidVelocity {
    // time_end が 0 の場合は、加速度・角加速度は変化しない。
    // 0 <= time <= time_end の制約から time も 0 となるため、初期状態をそのまま返す。
    if time_end == 0.0 {
        return rigid_body.clone();
    }

    // 加速度と角加速度の時間に対する変化率（傾き）を計算
    let accel_slope = (accel2 - accel1) / time_end;
    let angular_accel_slope = (angular_accel2 - angular_accel1) / time_end;

    // --- 1. 回転運動の計算 ---

    // 角速度 ω(t) = ω(0) + ∫[0,t] α(τ)dτ
    // α(τ) = angular_accel1 + angular_accel_slope * τ
    // ∫[0,t] α(τ)dτ = angular_accel1 * t + angular_accel_slope * t^2 / 2
    let new_angular_velocity = rigid_body.angular_velocity
        + angular_accel1 * time
        + angular_accel_slope * time.powi(2) / 2.0;

    // 角度 θ(t) = θ(0) + ∫[0,t] ω(τ)dτ
    // ω(τ) = ω(0) + angular_accel1 * τ + angular_accel_slope * τ^2 / 2
    // ∫[0,t] ω(τ)dτ = ω(0) * t + angular_accel1 * t^2 / 2 + angular_accel_slope * t^3 / 6
    let new_angle = rigid_body.angle
        + rigid_body.angular_velocity * time
        + angular_accel1 * time.powi(2) / 2.0
        + angular_accel_slope * time.powi(3) / 6.0;

    // --- 2. 並進運動の計算 ---

    // 仮定：加速度の方向は、剛体の初期角度 `rigid_body.angle` で固定
    let direction = Vec2::new(rigid_body.angle.cos(), rigid_body.angle.sin());

    // 速度 v(t) = v(0) + ∫[0,t] a(τ)dτ
    // a(τ) はベクトル。 a_vec(τ) = direction * (accel1 + accel_slope * τ)
    // 速度の変化量 Δv = ∫[0,t] a_vec(τ)dτ = direction * (accel1 * t + accel_slope * t^2 / 2)
    let velocity_change_scalar = accel1 * time + accel_slope * time.powi(2) / 2.0;
    let new_velocity = rigid_body.velocity + direction * velocity_change_scalar;

    // 位置 p(t) = p(0) + ∫[0,t] v(τ)dτ
    // v(τ) = v(0) + direction * (accel1 * τ + accel_slope * τ^2 / 2)
    // 位置の変化量 Δp = ∫[0,t] v(τ)dτ = v(0) * t + direction * (accel1 * t^2 / 2 + accel_slope * t^3 / 6)
    let position_change_from_accel_scalar =
        accel1 * time.powi(2) / 2.0 + accel_slope * time.powi(3) / 6.0;
    let new_position = rigid_body.position
        + rigid_body.velocity * time
        + direction * position_change_from_accel_scalar;

    // 計算結果を新しい RigidBody インスタンスとして返す
    RigidVelocity {
        position: new_position,
        velocity: new_velocity,
        angle: new_angle,
        angular_velocity: new_angular_velocity,
    }
}

// accel が時刻 0 から時刻 time_end にかけて accel1 から accel2 に変化する
// 0 < decay <= 1.0。速度は decay で減衰する
// angular_accel が時刻 0 から時刻 time_end にかけて angular_accel1 から angular_accel2 に変化する
// 0 < angular_decay <= 1.0。角速度は angular_decay で減衰する
// 0 <= time <= time_end とする。time 時点での各ステータスを求める
fn simulate_decay(
    rigid_body: &RigidVelocity,
    accel1: f64,
    accel2: f64,
    decay: f64,
    angular_accel1: f64,
    angular_accel2: f64,
    angular_decay: f64,
    time: f64,
    time_end: f64,
) -> RigidVelocity {
    if time_end == 0.0 {
        return rigid_body.clone();
    }

    // 加速度と角加速度の時間に対する変化率（傾き）を計算
    let accel_slope = (accel2 - accel1) / time_end;
    let angular_accel_slope = (angular_accel2 - angular_accel1) / time_end;

    // decay から減衰係数 k を計算
    // decay = exp(-k) => k = -ln(decay)
    // decay の値は (0, 1] の範囲にあることを想定
    let k_angular = -angular_decay.ln();
    let k_linear = -decay.ln();

    // --- 1. 回転運動の計算 ---
    let (new_angle, new_angular_velocity) = calculate_decay_final_state(
        rigid_body.angle,
        rigid_body.angular_velocity,
        angular_accel1,
        angular_accel_slope,
        k_angular,
        time,
    );

    // --- 2. 並進運動の計算 ---
    // 加速度の方向は剛体の初期角度で固定
    let direction = Vec2::new(rigid_body.angle.cos(), rigid_body.angle.sin());

    // 加速度ベクトル a(t) = direction * (accel1 + accel_slope * t)
    // a_vec(0) = direction * accel1
    let initial_accel_vec = direction * accel1;
    // a_vec_slope = direction * accel_slope
    let accel_slope_vec = direction * accel_slope;

    // x, y 成分をそれぞれ独立して計算
    let (new_pos_x, new_vel_x) = calculate_decay_final_state(
        rigid_body.position.x,
        rigid_body.velocity.x,
        initial_accel_vec.x,
        accel_slope_vec.x,
        k_linear,
        time,
    );
    let (new_pos_y, new_vel_y) = calculate_decay_final_state(
        rigid_body.position.y,
        rigid_body.velocity.y,
        initial_accel_vec.y,
        accel_slope_vec.y,
        k_linear,
        time,
    );

    // 計算結果を新しい RigidBody インスタンスとして返す
    RigidVelocity {
        position: Vec2::new(new_pos_x, new_pos_y),
        velocity: Vec2::new(new_vel_x, new_vel_y),
        angle: new_angle,
        angular_velocity: new_angular_velocity,
    }
}

/// 減衰を考慮した運動の解析解を計算するヘルパー関数
/// 運動方程式: x''(t) + k * x'(t) = a + b*t
///
/// # Arguments
/// * `initial_pos` - 初期位置 x(0)
/// * `initial_vel` - 初速度 x'(0)
/// * `a` - 加速度の初期値 a(0)
/// * `b` - 加速度の変化率 (slope)
/// * `k` - 減衰係数
/// * `t` - 経過時間
///
/// # Returns
/// * (`final_pos`, `final_vel`) - 時刻 t における位置と速度
fn calculate_decay_final_state(
    initial_pos: f64,
    initial_vel: f64,
    a: f64,
    b: f64,
    k: f64,
    t: f64,
) -> (f64, f64) {
    // k が非常に小さい場合は、減衰がないモデルとして計算する
    if k.abs() < 1e-9 {
        let final_vel = initial_vel + a * t + b * t.powi(2) / 2.0;
        let final_pos = initial_pos + initial_vel * t + a * t.powi(2) / 2.0 + b * t.powi(3) / 6.0;
        return (final_pos, final_vel);
    }

    let k2 = k * k;

    // --- 速度の計算 ---
    // 特解 v_p(t) = (b/k)t + a/k - b/k^2
    // v_p(0) = a/k - b/k^2
    let vp0 = a / k - b / k2;
    // 一般解 v(t) = (v(0) - v_p(0)) * exp(-k*t) + v_p(t)
    let final_vel = (initial_vel - vp0) * (-k * t).exp() + (a / k + b * t / k - b / k2);

    // --- 位置の計算 ---
    // Δx(t) = ∫[0,t] v(τ)dτ
    //        = (v(0) - v_p(0)) * ∫exp(-kτ)dτ + ∫v_p(τ)dτ
    // ∫exp(-kτ)dτ from 0 to t = (1 - exp(-k*t))/k = -expm1(-k*t)/k
    let pos_change_from_initial_vel = (initial_vel - vp0) * (-k * t).exp_m1() / (-k);

    // ∫v_p(τ)dτ from 0 to t = ∫( (b/k)τ + a/k - b/k^2 )dτ
    //                      = (b/(2k))t^2 + (a/k - b/k^2)t
    let pos_change_from_accel = (a / k - b / k2) * t + (b / (2.0 * k)) * t.powi(2);

    let final_pos = initial_pos + pos_change_from_initial_vel + pos_change_from_accel;

    (final_pos, final_vel)
}
