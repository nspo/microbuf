function val_bytes = from_big_endian(bytes)
%from_big_endian convert Big Endian bytes (uint8 array) to uint
% which represents the correct data.

if length(bytes)==2
    n=2;
    val_bytes = uint16(0);
    for i=1:n
        val_bytes = bitor(val_bytes, bitshift(uint16(bytes(i)), 8*(n-i)));
    end
elseif length(bytes)==4
    n=4;
    val_bytes = uint32(0);
    for i=1:n
        val_bytes = bitor(val_bytes, bitshift(uint32(bytes(i)), 8*(n-i)));
    end
elseif length(bytes)==8
    n=8;
    val_bytes = uint64(0);
    for i=1:n
        val_bytes = bitor(val_bytes, bitshift(uint64(bytes(i)), 8*(n-i)));
    end
end

end