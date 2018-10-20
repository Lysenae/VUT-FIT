using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using MqttService.Models;
using System.Threading.Tasks;

namespace WebApp.Controllers
{
    public class McuController : ApiController
    {
        private static MqttService.MqttService srv = new MqttService.MqttService();

        private async Task<MqttService.MqttService> GetMqttService()
        {
            if (!srv.GetClientConnected())
                await srv.ConnectClient();
            
            return srv;
        }

        [HttpGet]
        [ActionName("Read")]
        public async Task<List<Microcontroller>> GetAllMcus()
        {
            MqttService.MqttService lService = await GetMqttService();
            List<Microcontroller> result = lService.GetMicrocontrollers().ToList();
            return result;
            
        }

        [HttpGet]
        [ActionName("Read")]
        public async Task<IHttpActionResult> GetMcu(string id)
        {
            MqttService.MqttService lService = await GetMqttService();
            var mcus = lService.GetMicrocontrollers().ToList();
            var mcu = mcus.FirstOrDefault((p) => p.DeviceId == id);
            if (mcu == null)
            {
                return NotFound();
            }
            return Ok(mcu);
        }

        [HttpPost]
        [ActionName("Poweroff")]
        public async Task<IHttpActionResult> PostPowerOff(string id)
        {
            MqttService.MqttService lService = await GetMqttService();
            lService.CommandMcuPower(id, 0);
            return Ok(200);
        }

        [HttpPost]
        [ActionName("Poweron")]
        public async Task<IHttpActionResult> PostPowerOn(string id)
        {
            MqttService.MqttService lService = await GetMqttService();
            lService.CommandMcuPower(id, 1);
            return Ok(200);
        }

        [HttpPost]
        [ActionName("Kill")]
        public async Task<IHttpActionResult> PostKill(string id)
        {
            MqttService.MqttService lService = await GetMqttService();
            lService.CommandMcuHardShutdown(id);
            return Ok(200);
        }

        [HttpPost]
        [ActionName("Restart")]
        public async Task<IHttpActionResult> PostRestart(string id)
        {
            MqttService.MqttService lService = await GetMqttService();
            lService.CommandMcuReset(id);
            return Ok(200);
        }
    }
}
