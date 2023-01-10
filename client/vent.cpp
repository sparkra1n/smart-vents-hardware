class vent
{
    private:
        double temp;
        double heatingTime;
    
    vent(double temperature, double openDuration) : temp(temperature), heatingTime(openDuration) {}

    void setTemp(const double& t)
    {
        temp = t;
    }

    void setOpenDuration(const double& d)
    {
        heatingTime = d;
    }

    double getTemp(const double& t) const
    {
        return temp;
    }

    double getHeatingTime(const double& d) const
    {
        return heatingTime;
    }
};
