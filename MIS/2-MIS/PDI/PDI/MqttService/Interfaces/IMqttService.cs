using System;
using System.ServiceModel;
using System.Collections.Generic;
using MqttService.Models;
using System.Threading.Tasks;

namespace MqttService.Interfaces
{
    [ServiceContract]
    public interface IMqttService : IDisposable
    {
        [OperationContract]
        IEnumerable<PowerStrip> GetPowerStrips();

        [OperationContract]
        IEnumerable<Microcontroller> GetMicrocontrollers();

        [OperationContract]
        void CommandMcuPower(string deviceId, int value);

        [OperationContract]
        void CommandMcuHardShutdown(string deviceId);

        [OperationContract]
        void CommandMcuReset(string deviceId);

        [OperationContract]
        void CommandStripPower(string deviceId, int value);

        [OperationContract]
        bool GetClientConnected();

        [OperationContract]
        Task ConnectClient();
    }
}
