<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class CreateIisPoruchaTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('iis_porucha', function(Blueprint $table)
		{
			$table->integer('poruchaID', true);
			$table->integer('k_oprave')->index('poskozeno');
			$table->integer('technik')->nullable()->index('opravil');
			$table->integer('oznamujici')->index('oznamil');
			$table->string('popis')->nullable();
			$table->timestamp('cas_oznam')->default(DB::raw('CURRENT_TIMESTAMP'));
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::drop('iis_porucha');
	}

}
