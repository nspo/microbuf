disp('Executing MATLAB microbuf test: TestMessage1 deserialization');

addpath(fullfile(fileparts(mfilename('fullpath')), "/../../matlab"));
addpath(fullfile(fileparts(mfilename('fullpath')), "/../../output"));

BINARY_DATA_IN_FILE = getenv('BINARY_DATA_IN_FILE');

disp('Expecting serialized binary data in file: ');
disp(BINARY_DATA_IN_FILE);

fileID = fopen(BINARY_DATA_IN_FILE);
bytes = uint8(fread(fileID));

[err, bool_val, uint8_val, uint16_val, uint32_val, uint64_val, float32_arr_val, float64_arr_val] = deserialize_TestMessage1(bytes, length(bytes));
if err || bool_val ~= true ||...
    ~all([uint8_val uint16_val uint32_val uint64_val] == 123) ||...
    ~all(float32_arr_val == 0:9) || ~all(float64_arr_val == 0:9)
    error('Error deserializing message');
end

fclose(fileID);

disp('All tests passed!');
