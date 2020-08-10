function [crc] = crc16_aug_ccitt(bytes, bytes_length)
%CRC16_AUG_CCITT Calculate CRC16/AUG-CCITT checksum
%   width=16 poly=0x1021 init=0x1d0f refin=false refout=false xorout=0x0000
%   check=0xe5cc residue=0x0000 name="CRC-16/SPI-FUJITSU"

% similar to C example in https://stackoverflow.com/a/50857431/997151

crc = uint16(hex2dec('1d0f'));
for i=1:bytes_length
    crc = bitxor(crc, bitshift(uint16(bytes(i)), 8)); % crc = crc ^ (bytes(i) << 8)
    for j=1:8
        if bitand(crc, hex2dec('8000')) > 0 % if(crc & 0x8000)
            crc = bitxor(bitshift(crc, 1), hex2dec('1021')); % crc = (crc << 1) ^ 0x1021
        else
            crc = bitshift(crc, 1); % crc = crc << 1
        end
    end
end

end

