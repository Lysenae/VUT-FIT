using System;
using System.Collections.Generic;
using MqttService.Models;

namespace MqttService.Interfaces
{
    public interface IPowerStripRepository : IDisposable
    {
        void Add(PowerStrip powerstrip);
        IEnumerable<PowerStrip> All();
        void Delete(PowerStrip powerstrip);
        void DeleteAll();
        bool Contains(string deviceId);
        int Count();
        void Update(string deviceId, bool powered);
        void Update(string deviceId, double energyConsumption, string date);
        string FirstId();
    }
}
