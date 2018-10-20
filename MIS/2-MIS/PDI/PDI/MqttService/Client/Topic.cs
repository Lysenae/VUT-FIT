using System.Collections.Generic;
using System.Linq;
using MqttService.Logging;

namespace MqttService.Client
{
    public static class Topic
    {
        // MCU related topics:
        private const string MCU_ROOT       = "mcu";
        private const string MCU_ANNOUNCE   = "/conn";
        private const string MCU_STATUS     = "/status";
        private const string MCU_TEMP       = "/temp";
        private const string MCU_STATE      = "/state";
        private const string MCU_HSHUT      = "/hshut";
        private const string MCU_RESET      = "/reset";

        // Power strip related topics:
        private const string STRIP_ROOT     = "strip";
        private const string STRIP_ANNOUNCE = "/+/tele/INFO1";
        private const string STRIP_CPOWER   = "/cmnd/power";
        private const string STRIP_SPOWER   = "/stat/Power";
        private const string STRIP_ENERGY   = "/tele/ENERGY";

        private static string McuTopicFromDeviceId(string devId, string topic)
        {
            return "/" + MCU_ROOT + "/" + devId + topic;
        }

        private static string StripTopicFromDeviceId(string deviceId, string topic)
        {
            return "/" + STRIP_ROOT + "/" + deviceId + topic;
        }

        public static string McuAnnounce()
        {
            return MCU_ANNOUNCE;
        }

        public static string McuStatus(string devId)
        {
            return McuTopicFromDeviceId(devId, MCU_STATUS);
        }

        public static string McuTemperature(string devId)
        {
            return McuTopicFromDeviceId(devId, MCU_TEMP);
        }

        public static string McuState(string devId)
        {
            return McuTopicFromDeviceId(devId, MCU_STATE);
        }

        public static string McuHardShutdown(string devId)
        {
            return McuTopicFromDeviceId(devId, MCU_HSHUT);
        }

        public static string McuReset(string devId)
        {
            return McuTopicFromDeviceId(devId, MCU_RESET);
        }

        private static bool IsMcuMessage(string topic, string msg)
        {
            Logger.Info(topic);
            List<string> parts = topic.Split('/').ToList();
            foreach (var s in parts) Logger.Info(s);
            if (parts.Count == 4 &&  ("/" + parts[3]) == msg)
                return true;
            return false;
        }

        public static bool IsMcuStatus(string topic)
        {
            return IsMcuMessage(topic, MCU_STATUS);
        }

        public static bool IsMcuState(string topic)
        {
            return IsMcuMessage(topic, MCU_STATE);
        }

        public static bool IsMcuTemp(string topic)
        {
            return IsMcuMessage(topic, MCU_TEMP);
        }

        public static string StripAnnounce()
        {
            return "/" + STRIP_ROOT + STRIP_ANNOUNCE;
        }

        public static string StripPowerStatus(string devId)
        {
            return StripTopicFromDeviceId(devId, STRIP_SPOWER);
        }

        public static string StripEnergy(string devId)
        {
            return StripTopicFromDeviceId(devId, STRIP_ENERGY);
        }

        public static string StripPowerCommand(string devId)
        {
            return StripTopicFromDeviceId(devId, STRIP_CPOWER);
        }

        public static bool IsPowerStripMessage(string topic, string msg1, string msg2)
        {
            List<string> parts = topic.Split('/').ToList();
            if (parts.Count == 5 && parts[1] == STRIP_ROOT && parts[3] == msg1 && parts[4] == msg2)
                return true;
            return false;
        }

        public static bool IsPowerStripAnnounce(string topic)
        {
            List<string> p = StripAnnounce().Split('/').ToList();
            return IsPowerStripMessage(topic, p[3], p[4]);
        }

        public static bool IsPowerStripPowerCommand(string topic)
        {
            List<string> p = StripPowerCommand("id").Split('/').ToList();
            return IsPowerStripMessage(topic, p[3], p[4]);
        }

        public static bool IsPowerStripPowerStatus(string topic)
        {
            List<string> p = StripPowerStatus("id").Split('/').ToList();
            return IsPowerStripMessage(topic, p[3], p[4]);
        }

        public static bool IsPowerStripEnergy(string topic)
        {
            List<string> p = StripEnergy("id").Split('/').ToList();
            return IsPowerStripMessage(topic, p[3], p[4]);
        }

        public static string McuIdFromTopic(string topic)
        {
            List<string> parts = topic.Split('/').ToList();
            if (parts.Count == 4 && parts[1] == MCU_ROOT)
                return parts[2];
            return null;
        }

        public static string PowerStripIdFromTopic(string topic)
        {
            List<string> parts = topic.Split('/').ToList();
            if (parts.Count == 5 && parts[1] == STRIP_ROOT)
                return parts[2];
            return null;
        }
    }
}
