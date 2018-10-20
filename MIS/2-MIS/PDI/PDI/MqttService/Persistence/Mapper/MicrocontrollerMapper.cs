using System.Collections.Generic;
using System.Linq;
using MqttService.Models;
using MqttService.Persistence.Entity;

namespace MqttService.Persistence.Mapper
{
    internal class MicrocontrollerMapper
    {
        public MicrocontrollerEntity Map(Microcontroller mc)
        {
            return new MicrocontrollerEntity
            {
                DeviceId    = mc.DeviceId,
                Powered     = mc.Powered,
                Temperature = mc.Temperature
            };
        }

        public IEnumerable<Microcontroller> Map(IEnumerable<MicrocontrollerEntity> microcontrollers)
        {
            return microcontrollers.Select(
                mc => new Microcontroller(mc.DeviceId, mc.Powered, mc.Temperature)
            ).ToArray();
        }
    }
}
