disp('Executing MATLAB microbuf test: TestMessage1 (C++ tests need to have run before this! Expecting "TestMessage1_serialized.m" in build/ folder.)');

addpath(fullfile(fileparts(mfilename('fullpath')), "/../../matlab"));
addpath(fullfile(fileparts(mfilename('fullpath')), "/../../output"));

disp('- Deserialization');

% read in data which cpp test in test_TestMessage1.cpp generated
run('../../build/TestMessage1_serialized.m');

[err, bool_val, uint8_val, uint16_val, uint32_val, uint64_val, float32_arr_val, float64_arr_val] = deserialize_TestMessage1(bytes, length(bytes));

if err || bool_val ~= true ||...
    uint8_val ~= 123 || uint16_val ~= 123 || uint32_val ~= 123 || uint64_val ~= 123 || ...
    ~all(float32_arr_val == [0:9]) || ~all(float64_arr_val == [0:9])
    error('Error: TestMessage1 deserialization');
end


disp('All tests passed!');
