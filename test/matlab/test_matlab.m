disp('Executing MATLAB microbuf deserialization tests');

addpath('../../matlab');

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

disp('All tests passed!');
