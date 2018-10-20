<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class AddForeignKeysToIisPoruchaTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::table('iis_porucha', function(Blueprint $table)
		{
			$table->foreign('technik', 'opravil')->references('zamID')->on('iis_zamestnanec')->onUpdate('RESTRICT')->onDelete('RESTRICT');
			$table->foreign('oznamujici', 'oznamil')->references('zamID')->on('iis_zamestnanec')->onUpdate('RESTRICT')->onDelete('RESTRICT');
			$table->foreign('k_oprave', 'poskozeno')->references('zarID')->on('iis_zarizeni')->onUpdate('RESTRICT')->onDelete('RESTRICT');
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::table('iis_porucha', function(Blueprint $table)
		{
			$table->dropForeign('opravil');
			$table->dropForeign('oznamil');
			$table->dropForeign('poskozeno');
		});
	}

}
