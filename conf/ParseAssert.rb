##===- conf/ParseAssert.rb ------------------------------------------===##
##
## Copyright (C) 2023 Eightfold
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
##     limitations under the License.
##
##===----------------------------------------------------------------===##
##
##  Extracts relevant info from `assert.h` and generates a macro body.
##
##===----------------------------------------------------------------===##

def LoadFile
    file_handle = File.new("#{ARGV[0]}", 'r')
    return file_handle.read
end

def AssertScan(str)
    matches = str.scan(/^\s*#\s*define\s+(assert[^\n\\]*(?:\\\n[^\n\\]*)*)/m)
    return matches.map { |match| match.last.gsub(/\s*\\\n\s*/m, ' ') }
end

def ExtractArgNames(defs)
    matches = defs.map { |str| str.match(/^assert\(\s*(\w+)\s*\)/)[1] }
    return matches
end

def ExtractMacroBodies(defs)
    matches = defs.map { |str| str.match(/^assert\(\s*\w+\s*\)\s*(.*)$/)[1] }
    return matches
end

def RemoveVoidElements(args, bodies)
    return bodies.zip(args).reject { |body, arg| !body.include? arg }
end

def ParseFuncData(nonempty)
    return nonempty.map { |body, arg| [body.match(/(__?assert.*?)\s*\(/), arg, body] }
        .map { |name, arg, body| unless name.nil? then [name[1], arg, body] end }
end

def GenerateDefinition(data, args)
    new_arg, str_arg = args
    name, arg, body = data
    return body.sub(/#\s*#{arg}/m, str_arg).sub(arg, new_arg)
end


file_data = LoadFile()
defines = AssertScan(file_data)
args = ExtractArgNames(defines)
bodies = ExtractMacroBodies(defines)

nonempty = RemoveVoidElements(args, bodies)
func_data = ParseFuncData(nonempty).compact().first
print GenerateDefinition(func_data, ['chk', 'str'])
