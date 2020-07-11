function [is_big_endian] = is_big_endian()
%IS_BIG_ENDIAN Return whether system/target system is Big Endian
test_val = typecast(uint16(1), 'uint8');
is_big_endian = test_val(2)==1;

end

