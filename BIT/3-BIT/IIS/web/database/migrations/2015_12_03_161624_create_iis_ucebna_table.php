<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class CreateIisUcebnaTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('iis_ucebna', function(Blueprint $table)
		{
			$table->integer('roomID')->primary();
			$table->integer('mist_k_sezeni');
			$table->integer('pocet_rad')->nullable();
			$table->integer('mist_na_radu')->nullable();
			$table->integer('bloky')->nullable();
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::drop('iis_ucebna');
	}

}
