class Vent
{
    private:
        double temp;
        double heatingTime;
    public:
        vent(double temperature, double openDuration) : temp(temperature), heatingTime(openDuration) {}
        void setTemp(const double& t);
        void setOpenDuration(const double& d);
        double getTemp() const;
        double getHeatingTime() const;
        void open();
        void close();
};