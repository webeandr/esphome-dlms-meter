#include "esphome.h"
#if defined(ESP32)
#include "mbedtls/gcm.h"
#endif

static const char* ESPDM_VERSION = "0.9.0";
static const char* TAG = "espdm";

namespace esphome
{
    namespace espdm
    {
        class DlmsMeter : public Component, public uart::UARTDevice
        {
            public:
                DlmsMeter(uart::UARTComponent *parent);

                void setup() override;
                void loop() override;

                void set_voltage_sensors(sensor::Sensor *voltage_l1, sensor::Sensor *voltage_l2, sensor::Sensor *voltage_l3);
                void set_current_sensors(sensor::Sensor *current_l1, sensor::Sensor *current_l2, sensor::Sensor *current_l3);

                void set_active_power_sensors(sensor::Sensor *active_power_plus, sensor::Sensor *active_power_minus);
                void set_active_energy_sensors(sensor::Sensor *active_energy_plus, sensor::Sensor *active_energy_minus);
                void set_reactive_energy_sensors(sensor::Sensor *reactive_energy_plus, sensor::Sensor *reactive_energy_minus);
                void set_timestamp_sensor(text_sensor::TextSensor *timestamp);
                void set_evnspecial_sensor(sensor::Sensor *power_factor, text_sensor::TextSensor *meternumber);
                void enable_mqtt(mqtt::MQTTClientComponent *mqtt_client, const char *topic);

                void set_key(uint8_t key[], size_t keyLength);

            private:
                std::vector<uint8_t> receiveBuffer; // Stores the packet currently being received
                unsigned long lastRead = 0; // Timestamp when data was last read
                int readTimeout = 100; // Time to wait after last byte before considering data complete

                uint8_t key[16]; // Stores the decryption key
                size_t keyLength; // Stores the decryption key length (usually 16 bytes)

                const char *topic; // Stores the MQTT topic

#if defined(ESP32)
                mbedtls_gcm_context aes; // AES context used for decryption
#endif

                sensor::Sensor *voltage_l1 = NULL; // Voltage L1
                sensor::Sensor *voltage_l2 = NULL; // Voltage L2
                sensor::Sensor *voltage_l3 = NULL; // Voltage L3

                sensor::Sensor *current_l1 = NULL; // Current L1
                sensor::Sensor *current_l2 = NULL; // Current L2
                sensor::Sensor *current_l3 = NULL; // Current L3

                sensor::Sensor *active_power_plus = NULL; // Active power taken from grid
                sensor::Sensor *active_power_minus = NULL; // Active power put into grid

                sensor::Sensor *active_energy_plus = NULL; // Active energy taken from grid
                sensor::Sensor *active_energy_minus = NULL; // Active energy put into grid

                sensor::Sensor *reactive_energy_plus = NULL; // Reactive energy taken from grid
                sensor::Sensor *reactive_energy_minus = NULL; // Reactive energy put into grid

                text_sensor::TextSensor *timestamp = NULL; // Text sensor for the timestamp value

// EVN Special
                sensor::Sensor *power_factor = NULL; // Power Factor
                text_sensor::TextSensor *meternumber = NULL; // Text sensor for the meterNumber value

                mqtt::MQTTClientComponent *mqtt_client = NULL;

                uint16_t swap_uint16(uint16_t val);
                uint32_t swap_uint32(uint32_t val);
                void log_packet(std::vector<uint8_t> data);
                void abort();
        };
    }
}
