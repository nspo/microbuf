disp('Executing MATLAB microbuf test: General serialization');

addpath(fullfile(fileparts(mfilename('fullpath')), "/../../matlab"));

disp('- array tests...');

disp('-- fixarray tests...');

if any(microbuf.gen_fixarray(5) ~= hex2dec('95'))
    error('fixarray err');
end

if any(microbuf.gen_fixarray(15) ~= hex2dec('9f'))
    error('fixarray err');
end

if any(microbuf.gen_fixarray(0) ~= hex2dec('90'))
    error('fixarray err');
end

disp('-- array16 tests...');


if any(microbuf.gen_array16(256) ~= uint8([hex2dec('dc') 1 0]))
    error('array16 err');
end

if any(microbuf.gen_array16(256*256 + 256) ~= uint8([hex2dec('dc') 255 255]))
    error('array16 err');
end


disp('-- array32 tests...');

if any(microbuf.gen_array32(16777216) ~= uint8([hex2dec('dd') 1 0 0 0]))
    error('array32 err');
end

disp('- uint tests...');

disp('-- uint8 tests...');

if any(microbuf.gen_uint8(hex2dec('ab')) ~= uint8([hex2dec('cc') hex2dec('ab')]))
    error('uint8 err');
end

disp('-- uint16 tests...');

if any(microbuf.gen_uint16(hex2dec('ab')) ~= uint8([hex2dec('cd') 0 hex2dec('ab')]))
    error('uint16 err');
end

if any(microbuf.gen_uint16(hex2dec('abab')) ~= uint8([hex2dec('cd') hex2dec('ab') hex2dec('ab')]))
    error('uint16 err');
end

if any(microbuf.gen_uint16(hex2dec('abcd')) ~= uint8([hex2dec('cd') hex2dec('ab') hex2dec('cd')]))
    error('uint16 err');
end


disp('-- uint32 tests...');

if any(microbuf.gen_uint32(hex2dec('ab')) ~= uint8([hex2dec('ce') 0 0 0 hex2dec('ab')]))
    error('uint32 err');
end

if any(microbuf.gen_uint32(hex2dec('ab000000')) ~= uint8([hex2dec('ce') hex2dec('ab') 0 0 0]))
    error('uint32 err');
end


disp('-- uint64 tests...');

if any(microbuf.gen_uint64(hex2dec('ab')) ~= uint8([hex2dec('cf') 0 0 0 0 0 0 0 hex2dec('ab')]))
    error('uint64 err');
end

if any(microbuf.gen_uint64(hex2dec('ab00000000000000')) ~= uint8([hex2dec('cf') hex2dec('ab') 0 0 0 0 0 0 0]))
    error('uint64 err');
end


disp('- bool tests...');

if any(microbuf.gen_bool(true) ~= uint8([hex2dec('c3')]))
    error('bool err');
end

if any(microbuf.gen_bool(false) ~= uint8([hex2dec('c2')]))
    error('bool err');
end


disp('- float tests...');

if any(microbuf.gen_float32(0.0) ~= uint8([hex2dec('ca') 0 0 0 0]))
    error('float err');
end

if any(microbuf.gen_float32(256.25) ~= uint8([hex2dec('ca') hex2dec('43')  hex2dec('80')  hex2dec('20')  hex2dec('00') ]))
    error('float err');
end

if any(microbuf.gen_float32(-512.125) ~= uint8([hex2dec('ca') hex2dec('c4')  hex2dec('00')  hex2dec('08')  hex2dec('00') ]))
    error('float err');
end

if any(microbuf.gen_float64(-2048.25) ~= uint8([hex2dec('cb') hex2dec('C0') hex2dec('A0') hex2dec('00')  hex2dec('80') hex2dec('00') hex2dec('00') hex2dec('00') hex2dec('00') ]))
    error('float err');
end


disp('All tests passed!');
