<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class CreateIisKancelarTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('iis_kancelar', function(Blueprint $table)
		{
			$table->integer('roomID')->primary();
			$table->integer('el_zasuvky');
			$table->integer('eth_zasuvky')->nullable();
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::drop('iis_kancelar');
	}

}
