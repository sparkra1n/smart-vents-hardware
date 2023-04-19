/*
 * Title   : Vent Source File for the Vent Side
 * File    : Vent.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/Vent.hpp"

static Servo s_servo = Servo(GPIO_NUM_17);
static Pir s_pir = Pir(GPIO_NUM_16);

static I2c s_i2c {I2C_NUM_0};
static BME280I2C s_bme280;

void Vent::packetReader(void* pvParameter)
{
    while(1)
    {
        MacAddress mac;
        Message msg;

        if (EspNow::read(mac, msg))
        {
            if (mac == SERVER)
                msg.action == 1 ? Vent::open() : Vent::close();
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void Vent::packetSender(void* pvParameter)
{
    while (1)
    {
        Message msg{};
        msg.tempc = s_bme280.GetTemperature();
        msg.hasMotion = s_pir.getStatus();
        msg.isOpen = Vent::isOpen;

        std::cout << "temp: " << msg.tempc << " motion: " << msg.hasMotion << " open:" << msg.isOpen << '\n';
        ESP_ERROR_CHECK(EspNow::send(SERVER, msg));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void Vent::open()
{
    s_servo.setAngle(180);
    Vent::isOpen = true;
}

void Vent::close()
{
    s_servo.setAngle(0);
    Vent::isOpen = false;
}

void Vent::init()
{
    s_servo.init();
    s_pir.init();
    Vent::close();
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    ESP_ERROR_CHECK(EspNow::init());
    ESP_ERROR_CHECK(EspNow::addPeer(SERVER));

    // Initialise the I2C
    s_i2c.InitMaster(I2C_SDA, I2C_SCL, I2C_CLK_SPEED_HZ, true, true);

    // Initialize the BME280I2C device
    s_bme280.InitI2c(&s_i2c, 0x76);
    s_bme280.Init();
    s_bme280.SetMode(1);
    s_bme280.SetConfigFilter(1);

    // Start sending and reading data with the server
    xTaskCreate(packetSender, "packet_sender", 2048, NULL, 10, NULL);
    xTaskCreate(packetReader, "packet_reader", 2048, NULL, 4, NULL);
}

void Vent::loop()
{

}