<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class AddForeignKeysToIisZamestnanecTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::table('iis_zamestnanec', function(Blueprint $table)
		{
			$table->foreign('prava', 'mel')->references('pravaID')->on('iis_prava')->onUpdate('RESTRICT')->onDelete('RESTRICT');
			$table->foreign('pracoviste', 'sidlil')->references('roomID')->on('iis_mistnost')->onUpdate('RESTRICT')->onDelete('RESTRICT');
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::table('iis_zamestnanec', function(Blueprint $table)
		{
			$table->dropForeign('mel');
			$table->dropForeign('sidlil');
		});
	}

}
