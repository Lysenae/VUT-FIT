<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class CreateIisOddeleniTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('iis_oddeleni', function(Blueprint $table)
		{
			$table->integer('oddID', true);
			$table->string('nazev', 50)->nullable();
			$table->string('popis')->nullable();
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::drop('iis_oddeleni');
	}

}
