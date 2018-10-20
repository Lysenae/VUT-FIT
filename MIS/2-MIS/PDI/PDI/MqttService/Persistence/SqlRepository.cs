using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MqttService.Persistence.Sql;

namespace MqttService.Persistence
{
    public class SqlRepository
    {
        public MicrocontrollerSqlRepository Microcontrollers { get; private set; }
        public PowerStripSqlRepository PowerStrips { get; private set; }
        private MqttServiceDbContext _DbContext { get; set; }

        public SqlRepository()
        {
            this._DbContext = new MqttServiceDbContext();
            this.PowerStrips = new PowerStripSqlRepository(this._DbContext);
            this.Microcontrollers = new MicrocontrollerSqlRepository(this._DbContext);
        }

        public void Dispose()
        {
            this._DbContext?.Dispose();
        }
    }
}
