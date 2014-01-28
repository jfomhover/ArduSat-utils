ArduSat-utils
=============

[non official] utility classes, examples and libraries proposed for the ArduSatSDK

*Libraries* :
- SamplingLib : samples a data serie by approximation, to reduce storage size
- SAT_AppStorageEMU : emulates the SAT_AppStorage class, outputs via Serial the data that is sent
- SAT_AppStorageEMUSD : emulates the SAT_AppStorage class, outputs via Serial + writes on a SD file the data that is sent
- SAT_DataLib : library that formats the content of a data-related message written to SAT_AppStorage, data is formatted as binary chunks (structures), a decoder can be used to transform that into CSV
- ZMCameraLib : a library + example sketches for handling ZM-type cameras (C439 is one of them).