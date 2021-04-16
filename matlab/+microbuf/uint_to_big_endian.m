function bytes = uint_to_big_endian(value)
%uint_to_big_endian convert uint8, uint16, uint32, or uint64
%                   to Big Endian uint8 array

if isa(value, 'uint8')
    N = 1;
elseif isa(value, 'uint16')
    N = 2;
elseif isa(value, 'uint32')
    N = 4;
elseif isa(value, 'uint64')
    N = 8;
else
    error('Invalid input datatype');
end

bytes = uint8(zeros(1, N));
for i=1:N
    bytes(N+1-i) = uint8(bitand(value, 255)); % extract 0x...FF position
    value = bitshift(value, -8); % get the next byte to 0x...FF position
end

end