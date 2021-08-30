#!/bin/bash

set -eu

ruby tools/include-guard-appender.rb
ruby tools/autotest.rb -j 2
ruby tools/buildtest.rb -j 2
ruby tools/gather_coverage.rb > /dev/null
ruby tools/refactor.rb

echo "presubmit complete!!"
