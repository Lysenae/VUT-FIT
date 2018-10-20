<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class AddForeignKeysToIisZarizeniTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::table('iis_zarizeni', function(Blueprint $table)
		{
			$table->foreign('umisteno', 'obsahovala')->references('roomID')->on('iis_mistnost')->onUpdate('RESTRICT')->onDelete('RESTRICT');
			$table->foreign('zodpovida', 'vlastnil')->references('zamID')->on('iis_zamestnanec')->onUpdate('RESTRICT')->onDelete('RESTRICT');
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::table('iis_zarizeni', function(Blueprint $table)
		{
			$table->dropForeign('obsahovala');
			$table->dropForeign('vlastnil');
		});
	}

}
