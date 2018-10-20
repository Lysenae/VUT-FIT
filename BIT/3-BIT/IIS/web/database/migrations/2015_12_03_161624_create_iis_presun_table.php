<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class CreateIisPresunTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('iis_presun', function(Blueprint $table)
		{
			$table->integer('presunID', true);
			$table->integer('zarizeni')->index('presunuto');
			$table->integer('zadatel')->index('zadal');
			$table->string('duvod')->nullable();
			$table->date('provedeno')->nullable();
			$table->timestamp('cas_zadosti')->default(DB::raw('CURRENT_TIMESTAMP'));
			$table->integer('odkud');
			$table->integer('kam');
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::drop('iis_presun');
	}

}
