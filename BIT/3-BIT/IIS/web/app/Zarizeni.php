<?php

namespace App;

use Illuminate\Database\Eloquent\Model;
use Validator;
use DB;
class Zarizeni extends Model
{
    /**
     * Get a validator for an incoming registration request.
     *
     * @param  array  $data
     * @return \Illuminate\Contracts\Validation\Validator
     */
    protected function validator(array $data)
    {
	    
	$rules = [
		'umisteno' => 'required|max:7',
		'zodpovida' => 'required|max:5',
		'znacka' => 'required|max:30',
		'model' => 'required|max:30',
		'cena' => 'max:10',
		'specifikace' => 'max:255',
		'stav' => 'max:100',
	    ];
	return Validator::make($data, $rules);
    }

    /**
     * Create a new user instance after a valid registration.
     *
     * @param  array  $data
     * @return User
     */
    public static function createZar(array $data)
    {
        return DB::table('iis_zarizeni')->insert([
            'umisteno' => $data['umisteno'],
            'zodpovida' => $data['zodpovida'],
            'model' => $data['model'],
	    'znacka' => $data['znacka'],
            'cena' => $data['cena'],
            'specifikace' => $data['specifikace'],
            'stav' => $data['stav'],
        ]);
    }


    public static function updateZar(array $data, $id)
    {
	return DB::table('iis_zarizeni')
		->where('zarID', $id)
		->update([
            'umisteno' => $data['umisteno'],
            'zodpovida' => $data['zodpovida'],
            'znacka' => $data['znacka'],
            'model' => $data['model'],
            'cena' => $data['cena'],
            'specifikace' => $data['specifikace'],
            'stav' => $data['stav'],
        ]);
    }
    public static function deleteZar($id){
	DB::table('iis_zarizeni')
	    ->where('zarID', $id)
	    ->update(['smazan' => 1]);
    }
}
