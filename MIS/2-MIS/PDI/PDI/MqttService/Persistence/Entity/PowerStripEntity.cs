using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace MqttService.Persistence.Entity
{
    public class PowerStripEntity
    {
        [Key, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public Guid Id { get; set; }
        public string DeviceId { get; set; }
        public bool Powered { get; set; }
        public double EnergyConsumption { get; set; }
        public string Date { get; set; }
    }
}
