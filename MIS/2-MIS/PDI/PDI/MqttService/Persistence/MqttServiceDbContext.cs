using System.Data.Entity;
using MqttService.Persistence.Entity;

namespace MqttService.Persistence
{
    public class MqttServiceDbContext : DbContext
    {
        public MqttServiceDbContext() : this("name=Database")
        {
        }

        public MqttServiceDbContext(string connectionString) : base(connectionString)
        {
            Database.SetInitializer<MqttServiceDbContext>(
                new DropCreateDatabaseIfModelChanges<MqttServiceDbContext>()
            );
        }

        public DbSet<MicrocontrollerEntity> Microcontrollers { get; set; }
        public DbSet<PowerStripEntity> PowerStrips { get; set; }
    }
}
