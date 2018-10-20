<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class CreateIisMistnostTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('iis_mistnost', function(Blueprint $table)
		{
			$table->integer('roomID')->primary();
			$table->integer('oddeleni')->index('patrila');
			$table->string('lokace', 30)->nullable();
			$table->integer('sirka')->nullable();
			$table->integer('delka')->nullable();
			$table->integer('vyska')->nullable();
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::drop('iis_mistnost');
	}

}
