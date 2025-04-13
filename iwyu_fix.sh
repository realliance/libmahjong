#!/bin/bash
iwyu-tool -p build/compile_commands.json | iwyu-fix-includes --nocomments --nosafe_headers --nokeep_iwyu_namespace_format
