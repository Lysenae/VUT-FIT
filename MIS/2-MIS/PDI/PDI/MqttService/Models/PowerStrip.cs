using System;
using MqttService.Logging;

namespace MqttService.Models
{
    public sealed class PowerStrip : Device
    {
        public double EnergyConsumption { get; set; }
        public string Date { get; set; }

        public PowerStrip(string id, bool powered = true,
        double energyConsumption = 0.0, string date = "")
        : base(id, powered)
        {
            EnergyConsumption = energyConsumption;
            Date              = date;
        }
    }
}
