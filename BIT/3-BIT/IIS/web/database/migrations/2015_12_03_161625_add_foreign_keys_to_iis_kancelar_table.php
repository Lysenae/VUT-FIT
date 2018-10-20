<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class AddForeignKeysToIisKancelarTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::table('iis_kancelar', function(Blueprint $table)
		{
			$table->foreign('roomID', 'FKKancelar')->references('roomID')->on('iis_mistnost')->onUpdate('RESTRICT')->onDelete('RESTRICT');
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::table('iis_kancelar', function(Blueprint $table)
		{
			$table->dropForeign('FKKancelar');
		});
	}

}
