disp('Executing MATLAB microbuf deserialization tests');

addpath(fullfile(fileparts(mfilename('fullpath')), "/../../matlab"));

disp('- array tests...');

disp('-- fixarray tests...');

bytes = uint8([hex2dec('95')]);
[idx, err] = microbuf.check_fixarray(bytes, length(bytes), 5, 1);
if idx ~= 2 || err
    error('fixarray err');
end

bytes = uint8([hex2dec('9f')]);
[idx, err] = microbuf.check_fixarray(bytes, length(bytes), 15, 1);
if idx ~= 2 || err
    error('fixarray err');
end

bytes = uint8([hex2dec('90')]);
[idx, err] = microbuf.check_fixarray(bytes, length(bytes), 0, 1);
if idx ~= 2 || err
    error('fixarray err');
end

bytes = uint8([hex2dec('95')]);
[idx, err] = microbuf.check_fixarray(bytes, length(bytes), 6, 1);
if ~err
    error('fixarray err');
end

disp('-- array16 tests...');

bytes = uint8([hex2dec('dc') 1 0]);
[idx, err] = microbuf.check_array16(bytes, length(bytes), 256, 1);
if idx ~= 4 || err
    error('array16 err');
end

bytes = uint8([hex2dec('dc') 1]);
[idx, err] = microbuf.check_array16(bytes, length(bytes), 256, 1);
if ~err
    error('array16 err');
end

disp('-- array32 tests...');

bytes = uint8([hex2dec('dd') 1 0 0 0]);
[idx, err] = microbuf.check_array32(bytes, length(bytes), 16777216, 1);
if idx ~= 6 || err
    error('array32 err');
end

bytes = uint8([hex2dec('dd') 1 0 0]);
[idx, err] = microbuf.check_array32(bytes, length(bytes), 16777216, 1);
if ~err
    error('array32 err');
end

disp('- uint tests...');

disp('-- uint8 tests...');

bytes = uint8([hex2dec('cc') hex2dec('ab')]);
[idx, err, value] = microbuf.parse_uint8(bytes, length(bytes), 1);
if idx ~= 3 || err || value ~= hex2dec('ab') || ~isa(value, 'uint8')
    error('uint8 err');
end

bytes = uint8([hex2dec('cc')]);
[idx, err, value] = microbuf.parse_uint8(bytes, length(bytes), 1);
if ~err || ~isa(value, 'uint8')
    error('uint8 err');
end

bytes = uint8([hex2dec('cd') 0]);
[idx, err, value] = microbuf.parse_uint8(bytes, length(bytes), 1);
if ~err || ~isa(value, 'uint8')
    error('uint8 err');
end

disp('-- uint16 tests...');

bytes = uint8([hex2dec('cd') 0 hex2dec('ab')]);
[idx, err, value] = microbuf.parse_uint16(bytes, length(bytes), 1);
if idx ~= 4 || err || value ~= hex2dec('ab') || ~isa(value, 'uint16')
    error('uint16 err');
end

bytes = uint8([hex2dec('cd') hex2dec('ab') hex2dec('ab')]);
[idx, err, value] = microbuf.parse_uint16(bytes, length(bytes), 1);
if idx ~= 4 || err || value ~= hex2dec('abab') || ~isa(value, 'uint16')
    error('uint16 err');
end

bytes = uint8([hex2dec('cd') hex2dec('ab') hex2dec('cd')]);
[idx, err, value] = microbuf.parse_uint16(bytes, length(bytes), 1);
if idx ~= 4 || err || value ~= hex2dec('abcd') || ~isa(value, 'uint16')
    error('uint16 err');
end

bytes = uint8([hex2dec('cd') 0]);
[idx, err, value] = microbuf.parse_uint16(bytes, length(bytes), 1);
if ~err || ~isa(value, 'uint16')
    error('uint16 err');
end


disp('-- uint32 tests...');

bytes = uint8([hex2dec('ce') 0 0 0 hex2dec('ab')]);
[idx, err, value] = microbuf.parse_uint32(bytes, length(bytes), 1);
if idx ~= 6 || err || value ~= hex2dec('ab') || ~isa(value, 'uint32')
    error('uint32 err');
end

bytes = uint8([hex2dec('ce') hex2dec('ab') 0 0 0]);
[idx, err, value] = microbuf.parse_uint32(bytes, length(bytes), 1);
if idx ~= 6 || err || value ~= hex2dec('ab000000') || ~isa(value, 'uint32')
    error('uint32 err');
end

bytes = uint8([hex2dec('cd') 0]);
[idx, err, value] = microbuf.parse_uint32(bytes, length(bytes), 1);
if ~err || ~isa(value, 'uint32')
    error('uint32 err');
end

disp('-- uint64 tests...');

bytes = uint8([hex2dec('cf') 0 0 0 0 0 0 0 hex2dec('ab')]);
[idx, err, value] = microbuf.parse_uint64(bytes, length(bytes), 1);
if idx ~= 10 || err || value ~= hex2dec('ab') || ~isa(value, 'uint64')
    error('uint64 err');
end

bytes = uint8([hex2dec('cf') hex2dec('ab') 0 0 0 0 0 0 0]);
[idx, err, value] = microbuf.parse_uint64(bytes, length(bytes), 1);
if idx ~= 10 || err || value ~= hex2dec('ab00000000000000') || ~isa(value, 'uint64')
    error('uint64 err');
end

bytes = uint8([hex2dec('cd') 0]);
[idx, err, value] = microbuf.parse_uint64(bytes, length(bytes), 1);
if ~err || ~isa(value, 'uint64')
    error('uint64 err');
end

disp('- bool tests...');

bytes = uint8([hex2dec('c3')]);
[idx, err, value] = microbuf.parse_bool(bytes, length(bytes), 1);
if idx ~= 2 || err || value ~= true || ~isa(value, 'logical')
    error('bool err');
end

bytes = uint8([hex2dec('c2')]);
[idx, err, value] = microbuf.parse_bool(bytes, length(bytes), 1);
if idx ~= 2 || err || value ~= false || ~isa(value, 'logical')
    error('bool err');
end

bytes = uint8([]);
[idx, err, value] = microbuf.parse_bool(bytes, length(bytes), 1);
if ~err || ~isa(value, 'logical')
    error('bool err');
end

disp('- CRC tests...');
if microbuf.crc16_aug_ccitt([], 0) ~= hex2dec('1d0f')
    error('CRC error');
end


if microbuf.crc16_aug_ccitt(uint8('A'), 1) ~= hex2dec('9479')
    error('CRC error');
end

if microbuf.crc16_aug_ccitt(uint8('123456789'), 9) ~= hex2dec('e5cc')
    error('CRC error');
end

if microbuf.crc16_aug_ccitt(uint8('1234567890'), 10) ~= hex2dec('57d8')
    error('CRC error');
end

if microbuf.crc16_aug_ccitt(repmat(uint8('A'), 1, 256), 256) ~= hex2dec('e938')
    error('CRC error');
end

if microbuf.check_crc(uint8([hex2dec('cd') hex2dec('1d') hex2dec('0f')]), 3, 1) == true
    error('CRC error');
end

disp('- float tests...');

bytes = uint8([hex2dec('ca') 0 0 0 0]);
[idx, err, value] = microbuf.parse_float32(bytes, length(bytes), 1);
if idx ~= 6 || err || value ~= 0.0 || ~isa(value, 'single')
    error('float err');
end

bytes = uint8([hex2dec('ca') hex2dec('43')  hex2dec('80')  hex2dec('20')  hex2dec('00') ]);
[idx, err, value] = microbuf.parse_float32(bytes, length(bytes), 1);
if idx ~= 6 || err || value ~= 256.25 || ~isa(value, 'single')
    error('float err');
end

bytes = uint8([hex2dec('ca') hex2dec('c4')  hex2dec('00')  hex2dec('08')  hex2dec('00') ]);
[idx, err, value] = microbuf.parse_float32(bytes, length(bytes), 1);
if idx ~= 6 || err || value ~= -512.125 || ~isa(value, 'single')
    error('float err');
end

bytes = uint8([hex2dec('cb') hex2dec('C0') hex2dec('A0') hex2dec('00')  hex2dec('80') hex2dec('00') hex2dec('00') hex2dec('00') hex2dec('00') ]);
[idx, err, value] = microbuf.parse_float64(bytes, length(bytes), 1);
if idx ~= 10 || err || value ~= -2048.25 || ~isa(value, 'double')
    error('float err');
end

disp('All tests passed!');
