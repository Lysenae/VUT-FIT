using System;
using System.Linq;
using System.Collections.Generic;
using System.Data.Entity;
using MqttService.Interfaces;
using MqttService.Models;
using MqttService.Persistence.Mapper;
using MqttService.Persistence.Entity;

namespace MqttService.Persistence.Sql
{
    public class PowerStripSqlRepository : IPowerStripRepository
    {
        private readonly PowerStripMapper _powerstripMapper = new PowerStripMapper();
        private readonly MqttServiceDbContext _mqqtSvcDbContext;

        public PowerStripSqlRepository(MqttServiceDbContext mqttSvcDbContext)
        {
            this._mqqtSvcDbContext = mqttSvcDbContext;
        }

        public void Add(PowerStrip powerstrip)
        {
            if (!Contains(powerstrip.DeviceId))
            {
                this._mqqtSvcDbContext.PowerStrips.Add(this._powerstripMapper.Map(powerstrip));
                this._mqqtSvcDbContext.SaveChanges();
            }
        }

        public IEnumerable<PowerStrip> All()
        {
            return this._powerstripMapper.Map(this._mqqtSvcDbContext.PowerStrips);
        }

        public void DeleteAll()
        {
            this._mqqtSvcDbContext.DeleteAll<PowerStripEntity>();
        }

        public void Delete(PowerStrip powerstrip)
        {
            var entry = _mqqtSvcDbContext.PowerStrips.Where(
                p => p.DeviceId == powerstrip.DeviceId).FirstOrDefault<PowerStripEntity>();
            if (entry != null)
            {
                _mqqtSvcDbContext.Entry(entry).State = EntityState.Deleted;
                _mqqtSvcDbContext.SaveChanges();
            }
        }

        public void Dispose()
        {
            this._mqqtSvcDbContext?.Dispose();
        }

        public bool Contains(string deviceId)
        {
            var entry = _mqqtSvcDbContext.PowerStrips.Where(
                p => p.DeviceId == deviceId).FirstOrDefault<PowerStripEntity>();
            return entry == null ? false : true;
        }

        public int Count()
        {
            return this._mqqtSvcDbContext.PowerStrips.ToList().Count();
        }

        public void Update(string deviceId, bool powered)
        {
            var entry = _mqqtSvcDbContext.PowerStrips.Where(
                p => p.DeviceId == deviceId).FirstOrDefault<PowerStripEntity>();
            if (entry != null)
            {
                entry.Powered = powered;
                _mqqtSvcDbContext.Entry(entry).State = EntityState.Modified;
                _mqqtSvcDbContext.SaveChanges();
            }
        }

        public void Update(string deviceId, double energyConsumption, string date)
        {
            var entry = _mqqtSvcDbContext.PowerStrips.Where(
                p => p.DeviceId == deviceId).FirstOrDefault<PowerStripEntity>();
            if (entry != null)
            {
                entry.EnergyConsumption = energyConsumption;
                entry.Date = date;
                _mqqtSvcDbContext.Entry(entry).State = EntityState.Modified;
                _mqqtSvcDbContext.SaveChanges();
            }
        }

        public string FirstId()
        {
            
            if (Count() > 0)
                return (this._mqqtSvcDbContext.PowerStrips.ToList()[0].DeviceId);
            return null;
        }
    }
}
