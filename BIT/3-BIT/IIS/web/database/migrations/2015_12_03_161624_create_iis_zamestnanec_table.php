<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class CreateIisZamestnanecTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('iis_zamestnanec', function(Blueprint $table)
		{
			$table->integer('zamID', true);
			$table->integer('rodnecislo');
			$table->integer('pracoviste')->index('sidlil');
			$table->integer('prava')->index('mel');
			$table->string('login', 10)->nullable()->unique('login');
			$table->string('jmeno', 20);
			$table->string('prijmeni', 20);
			$table->string('password', 60)->nullable();
			$table->date('datum_narozeni')->nullable();
			$table->integer('mobil')->nullable();
			$table->integer('telefon_kancelar')->nullable();
			$table->string('mail')->nullable();
			$table->boolean('aktivni')->default(1);
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::drop('iis_zamestnanec');
	}

}
