<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class CreateIisZarizeniTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('iis_zarizeni', function(Blueprint $table)
		{
			$table->integer('zarID', true);
			$table->integer('umisteno')->index('obsahovala');
			$table->integer('zodpovida')->index('vlastnil');
			$table->string('znacka', 30);
			$table->string('model', 30);
			$table->integer('cena')->nullable();
			$table->string('specifikace')->nullable();
			$table->string('stav', 100)->nullable();
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::drop('iis_zarizeni');
	}

}
