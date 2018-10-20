<?php

namespace App;

use Illuminate\Database\Eloquent\Model;
use Validator;
use DB;
class Mistnosti extends Model
{
    /**
     * Get a validator for an incoming registration request.
     *
     * @param  array  $data
     * @return \Illuminate\Contracts\Validation\Validator
     */
    protected function validator(array $data, $mode, $type, $id)
    {
	$rules = [
	    'oddeleni' => 'required',
	    'lokace' => 'max:30',
	    'sirka' => 'max:2',
	    'delka' => 'max:2',
	    'vyska' => 'max:2',
	];

	if ($type == 'kan')
	    $extension = [
		'el_zasuvky' => 'required|max:2',
		'eth_zasuvky' => 'max:2',
		];
	elseif ($type == 'uce')
	    $extension = [
		'mist_k_sezeni' => 'required|max:3',
		'pocet_rad' => 'max:2',
		'mist_na_radu' => 'max:3',
		'bloky' => 'max:2',
		];
	
	$rules = array_merge($rules, $extension);

    	return Validator::make($data, $rules);
    }

    public static function createMis(array $data, $type)
    {
	DB::table('iis_mistnost')->insert([
	    'roomID' => $data['roomID'],
            'oddeleni' => $data['oddeleni'],
            'lokace' => $data['lokace'],
            'sirka' => $data['sirka'],
            'delka' => $data['delka'],
            'vyska' => $data['vyska'],
        ]);

	if ($type == 'kan')
	    DB::table('iis_kancelar')->insert([
		'roomID' => $data['roomID'],
		'el_zasuvky' => $data['el_zasuvky'],
		'eth_zasuvky' => $data['eth_zasuvky'],
	    ]);
	elseif ($type == 'uce')
	    DB::table('iis_ucebna')->insert([
		'roomID' => $data['roomID'],
		'mist_k_sezeni' => $data['mist_k_sezeni'],
		'pocet_rad' => $data['pocet_rad'],
		'mist_na_radu' => $data['mist_na_radu'],
		'bloky' => $data['bloky'],
	    ]);
    }


    public static function updateMis(array $data, $id, $type)
    {
	DB::table('iis_mistnost')
		->where('roomID', $id)
		->update([
		    'oddeleni' => $data['oddeleni'],
		    'lokace' => $data['lokace'],
		    'sirka' => $data['sirka'],
		    'delka' => $data['delka'],
		    'vyska' => $data['vyska'],
		]);

	if ($type == 'kan')
	    DB::table('iis_kancelar')
		->where('roomID', $id)
		->update([
		    'el_zasuvky' => $data['el_zasuvky'],
		    'eth_zasuvky' => $data['eth_zasuvky'],
		]);
	elseif ($type == 'uce')
	    DB::table('iis_ucebna')
		->where('roomID', $id)
		->update([
		    'mist_k_sezeni' => $data['mist_k_sezeni'],
		    'pocet_rad' => $data['pocet_rad'],
		    'mist_na_radu' => $data['mist_na_radu'],
		    'bloky' => $data['bloky'],
		]);
    }
    public static function deleteMis($id,$type){
	DB::table('iis_mistnost')
	    ->where('roomID', $id)
	    ->update(['smazan' => 1]);
    }
}
