import js from '@eslint/js';

// TODO: Remove .eslintrc.json
export default [
    js.configs.recommended,
    {
        // files: [
        //     './target_src/javascript/**/*.js',
        // ],
        rules: {
            semi: ['error', 'always'],
            'prefer-const': 'error',
            'indent': [
                'error',
                4
            ],
            'linebreak-style': [
                'error',
                'unix'
            ],
            'quotes': [
                'error',
                'single'
            ],
            
        },
    }
];