<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class AddForeignKeysToIisMistnostTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::table('iis_mistnost', function(Blueprint $table)
		{
			$table->foreign('oddeleni', 'patrila')->references('oddID')->on('iis_oddeleni')->onUpdate('RESTRICT')->onDelete('RESTRICT');
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::table('iis_mistnost', function(Blueprint $table)
		{
			$table->dropForeign('patrila');
		});
	}

}
