using System.Linq;
using System.Collections.Generic;
using System.Data.Entity;
using MqttService.Interfaces;
using MqttService.Models;
using MqttService.Persistence.Mapper;
using MqttService.Persistence.Entity;

namespace MqttService.Persistence.Sql
{
    public class MicrocontrollerSqlRepository : IMicrocontrollerRepository
    {
        private MicrocontrollerMapper _microcontrollerMapper = new MicrocontrollerMapper();
        private MqttServiceDbContext _mqqtSvcDbContext;

        public MicrocontrollerSqlRepository(MqttServiceDbContext mqttSvcDbContext)
        {
            this._mqqtSvcDbContext = mqttSvcDbContext;
        }

        public void Add(Microcontroller microcontroller)
        {
            if (!Contains(microcontroller.DeviceId))
            {
                this._mqqtSvcDbContext.Microcontrollers.Add(this._microcontrollerMapper.Map(microcontroller));
                this._mqqtSvcDbContext.SaveChanges();
            }
        }

        public IEnumerable<Microcontroller> All()
        {
            this._mqqtSvcDbContext = new MqttServiceDbContext();
            return this._microcontrollerMapper.Map(this._mqqtSvcDbContext.Microcontrollers);
        }

        public void DeleteAll()
        {
            this._mqqtSvcDbContext.DeleteAll<MicrocontrollerEntity>();
        }

        public void Delete(Microcontroller microcontroller)
        {
            var entry = _mqqtSvcDbContext.Microcontrollers.Where(
                p => p.DeviceId == microcontroller.DeviceId).FirstOrDefault<MicrocontrollerEntity>();
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
            var entry = _mqqtSvcDbContext.Microcontrollers.Where(
                p => p.DeviceId == deviceId).FirstOrDefault<MicrocontrollerEntity>();
            return entry == null ? false : true;
        }

        public int Count()
        {
            return this._mqqtSvcDbContext.Microcontrollers.ToList().Count();
        }

        public void Update(string deviceId, bool powered)
        {
            var entry = _mqqtSvcDbContext.Microcontrollers.Where(
                p => p.DeviceId == deviceId).FirstOrDefault<MicrocontrollerEntity>();
            if (entry != null)
            {
                entry.Powered = powered;
                _mqqtSvcDbContext.Entry(entry).State = EntityState.Modified;
                _mqqtSvcDbContext.SaveChanges();
            }
        }

        public void Update(string deviceId, double temperature)
        {
            var entry = _mqqtSvcDbContext.Microcontrollers.Where(
                p => p.DeviceId == deviceId).FirstOrDefault<MicrocontrollerEntity>();
            if (entry != null)
            {
                entry.Temperature = temperature;
                _mqqtSvcDbContext.Entry(entry).State = EntityState.Modified;
                _mqqtSvcDbContext.SaveChanges();
            }
        }
    }
}
