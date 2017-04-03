# JSON Performance Considerations
DeepRTS focuses on high performance delivery of game states to allow for accelerated machine learning speeds.
Serialization and Deserialization are a big part of this, as the game logs numerous items to json
Since early release, the game used Nhumann JSON, but because of a relatively high requirement (C++11) and 
bad performance, DeepRTS decided to go over to RapidJSON which is one of the fastest parsers and serializers.
More about this can be read at the comprehensive test over at [miloyip's](https://github.com/miloyip/nativejson-benchmark) repository.
