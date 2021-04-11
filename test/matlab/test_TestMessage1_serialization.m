disp('Executing MATLAB microbuf test: TestMessage1 serialization');

addpath(fullfile(fileparts(mfilename('fullpath')), "/../../matlab"));
addpath(fullfile(fileparts(mfilename('fullpath')), "/../../output"));

BINARY_DATA_OUT_FILE = getenv('BINARY_DATA_OUT_FILE');

disp('Writing serialized binary data to file: ');
disp(BINARY_DATA_OUT_FILE);

% generate test data
bool_val = true;
uint8_val = uint8(123);
uint16_val = uint16(123);
uint32_val = uint32(123);
uint64_val = uint64(123);
float32_arr_val = single(0:9);
float64_arr_val = double(0:9);

fileID = fopen(BINARY_DATA_OUT_FILE, 'w');
bytes = serialize_TestMessage1(bool_val, uint8_val, uint16_val, uint32_val, uint64_val, float32_arr_val, float64_arr_val);
fwrite(fileID, bytes);
fclose(fileID);

disp('All tests passed!');
