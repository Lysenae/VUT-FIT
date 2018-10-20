namespace MqttService.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class date_to_string : DbMigration
    {
        public override void Up()
        {
            AlterColumn("dbo.PowerStripEntities", "Date", c => c.String());
        }
        
        public override void Down()
        {
            AlterColumn("dbo.PowerStripEntities", "Date", c => c.DateTime(nullable: false));
        }
    }
}
