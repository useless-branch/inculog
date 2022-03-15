//
// Created by patrick on 3/10/22.
//
#pragma once

struct SensorConfig{
    struct IncubatorBoard{
        static constexpr auto name{"Incubator"};
        static constexpr auto canBaseAddress{70};
        struct Sensors {
            struct Temperature {
            private:
                static constexpr auto canBlockOffsetTemp{0};
            public:
                static constexpr auto name{"Temperature"};
                static constexpr auto address{0x44};
                static constexpr auto canAddress{canBaseAddress + canBlockOffsetTemp};
            };
            struct Humidity{
                struct absolute{
                private:

                    static constexpr auto canBlockOffsetAbsoluteHumid{1};
                public:
                    static constexpr auto name{"AbsoluteHumid"};
                    static constexpr auto canAddress{
                            canBaseAddress + canBlockOffsetAbsoluteHumid};

                };
                struct relative{
                private:
                    static constexpr auto canBlockOffsetRelativeHumid{2};
                public:
                    static constexpr auto name{"RelativeHumid"};
                    static constexpr auto canAddress{
                            canBaseAddress + canBlockOffsetRelativeHumid};
                };
            };
            struct AirQuality {
            private:
                static constexpr auto canBlockOffsetVOC{3};
                static constexpr auto canBlockOffsetCO2Eq{4};

            public:
                static constexpr auto name{"AirQuality"};
                static constexpr auto address{0x58};
                struct VOC{
                    static constexpr auto name{"VOC"};
                    static constexpr auto canAddress{canBaseAddress + canBlockOffsetVOC};
                };
                struct CO2Eq{
                    static constexpr auto name{"CO2Equivalent"};
                    static constexpr auto canAddress{canBaseAddress + canBlockOffsetCO2Eq};
                };

            };
            struct Pressure {
            private:
                static constexpr auto canBlockAddress{5};

            public:
                static constexpr auto name{"Pressure"};
                static constexpr auto address{0x77};
                static constexpr auto canAddress{canBaseAddress + canBlockAddress};
            };
            struct Light {
            private:
                static constexpr auto canBlockAddress{6};

            public:
                static constexpr auto name{"Light"};
                static constexpr auto address{0x23};
                static constexpr auto canAddress{canBaseAddress + canBlockAddress};
            };
        };
    };

    struct FridgeBoard_1{
        static constexpr auto name{"Fridge_1"};
        static constexpr auto canBaseAddress{77};
        struct Sensors {
            struct Temperature {
            private:
                static constexpr auto canBlockOffsetTemp{0};
            public:
                static constexpr auto name{"Temperature"};
                static constexpr auto address{0x44};
                static constexpr auto canAddress{canBaseAddress + canBlockOffsetTemp};
            };
            struct Humidity{
                struct absolute{
                private:

                    static constexpr auto canBlockOffsetAbsoluteHumid{1};
                public:
                    static constexpr auto name{"AbsoluteHumid"};
                    static constexpr auto canAddress{
                            canBaseAddress + canBlockOffsetAbsoluteHumid};

                };
                struct relative{
                private:
                    static constexpr auto canBlockOffsetRelativeHumid{2};
                public:
                    static constexpr auto name{"RelativeHumid"};
                    static constexpr auto canAddress{
                            canBaseAddress + canBlockOffsetRelativeHumid};
                };
            };
            struct AirQuality {
            private:
                static constexpr auto canBlockOffsetVOC{3};
                static constexpr auto canBlockOffsetCO2Eq{4};

            public:
                static constexpr auto name{"AirQuality"};
                static constexpr auto address{0x58};
                struct VOC{
                    static constexpr auto name{"VOC"};
                    static constexpr auto canAddress{canBaseAddress + canBlockOffsetVOC};
                };
                struct CO2Eq{
                    static constexpr auto name{"CO2Equivalent"};
                    static constexpr auto canAddress{canBaseAddress + canBlockOffsetCO2Eq};
                };

            };
            struct Pressure {
            private:
                static constexpr auto canBlockAddress{5};

            public:
                static constexpr auto name{"Pressure"};
                static constexpr auto address{0x77};
                static constexpr auto canAddress{canBaseAddress + canBlockAddress};
            };
            struct Light {
            private:
                static constexpr auto canBlockAddress{6};

            public:
                static constexpr auto name{"Light"};
                static constexpr auto address{0x23};
                static constexpr auto canAddress{canBaseAddress + canBlockAddress};
            };
        };
    };
};
