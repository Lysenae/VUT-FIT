<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;

class AddForeignKeysToIisUcebnaTable extends Migration {

	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::table('iis_ucebna', function(Blueprint $table)
		{
			$table->foreign('roomID', 'FKUcebna')->references('roomID')->on('iis_mistnost')->onUpdate('RESTRICT')->onDelete('RESTRICT');
		});
	}


	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::table('iis_ucebna', function(Blueprint $table)
		{
			$table->dropForeign('FKUcebna');
		});
	}

}
