<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class AddForeignKeysToIisPresunTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::table('iis_presun', function(Blueprint $table)
		{
			$table->foreign('zarizeni', 'presunuto')->references('zarID')->on('iis_zarizeni')->onUpdate('RESTRICT')->onDelete('RESTRICT');
			$table->foreign('zadatel', 'zadal')->references('zamID')->on('iis_zamestnanec')->onUpdate('RESTRICT')->onDelete('RESTRICT');
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::table('iis_presun', function(Blueprint $table)
		{
			$table->dropForeign('presunuto');
			$table->dropForeign('zadal');
		});
	}

}
