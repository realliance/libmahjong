#!/bin/bash
iwyu-tool -p build/compile_commands.json | iwyu-fix-includes --nocomments
