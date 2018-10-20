<?php

namespace App;

use Illuminate\Database\Eloquent\Model;
use Validator;
use DB;
class Oddeleni extends Model
{
    /**
     * Get a validator for an incoming registration request.
     *
     * @param  array  $data
     * @return \Illuminate\Contracts\Validation\Validator
     */
    protected function validator(array $data, $mode,$id)
    {
	if ($mode == 'create')
	    $validator = Validator::make($data, [
		'nazev' => 'required|max:50|unique:iis_oddeleni',
		'popis' => 'required|max:255',
	    ]);
	elseif ($mode == 'update'){
	    $rules=[ 
		'nazev' => 'required|max:50|unique:iis_oddeleni,nazev,',
		'popis' => 'required|max:255',
	    ];
	    $rules['nazev'] = $rules['nazev']. $id . ',oddID';
	    
	    $validator = Validator::make($data, $rules);
	}
	return $validator;
    }

    /**
     * Create a new user instance after a valid registration.
     *
     * @param  array  $data
     * @return User
     */
    public static function createOdd(array $data)
    {
        return DB::table('iis_oddeleni')->insert([
            'nazev' => $data['nazev'],
            'popis' => $data['popis'],
        ]);
    }


    public static function updateOdd(array $data, $id)
    {
	return DB::table('iis_oddeleni')
		->where('oddID', $id)
		->update([
            'popis' => $data['popis'],
        ]);
    }
    public static function deleteOdd($id){
	DB::table('iis_oddeleni')
	    ->where('oddID', $id)
	    ->update(['smazan' => 1]);
    }
}
