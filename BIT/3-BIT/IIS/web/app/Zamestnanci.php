<?php

namespace App;

use DB;
use Hash;
use Illuminate\Database\Eloquent\Model;
use Validator;
class Zamestnanci extends Model
{
    /**
     * Get a validator for an incoming registration request.
     *
     * @param  array  $data
     * @return \Illuminate\Contracts\Validation\Validator
     */
    
    protected function validator(array $data, $mode)
    {
	if ($mode == 'create')
	    $validator = Validator::make($data, [
		'pracoviste' => 'required|max:7',
		'pozice' => 'required',
		'login' => 'required|max:10|unique:iis_zamestnanec',
		'jmeno' => 'required|max:20',
		'prijmeni' => 'required|max:20',
		'password' => 'required|confirmed|min:6',
		'datum_narozeni' => 'date',
		'mobil' => 'digits:12',
		'telefon_kancelar' => 'digits:12',
		'mail' => 'email|unique:iis_zamestnanec',
	    ]);
	elseif ($mode == 'update')
	    $validator = Validator::make($data, [
                'pracoviste' => 'required|max:7',
		'pozice' => 'required',
		'jmeno' => 'required|max:20',
		'prijmeni' => 'required|max:20',
		'datum_narozeni' => 'date',
		'mobil' => 'digits:12',
		'telefon_kancelar' => 'digits:12',
		'mail' => 'email',
	    ]);

	return $validator;
    }

    /**
     * Create a new user instance after a valid registration.
     *
     * @param  array  $data
     * @return User
     */
    public $timestamps = false;
    public static function createZam(array $data)
    {
        DB::table('iis_zamestnanec')->insert([
            'pracoviste' => $data['pracoviste'],
            'prava' => $data['pozice'],
            'login' => $data['login'],
            'jmeno' => $data['jmeno'],
            'prijmeni' => $data['prijmeni'],
            'password' => Hash::make($data['password']),
            'datum_narozeni' => $data['datum_narozeni'],
            'mobil' => $data['mobil'],
            'telefon_kancelar' => $data['telefon_kancelar'],
            'mail' => $data['mail'],
	]);
	User::create(['login' => $data['login'], 'password' => bcrypt($data['password']), 'prava' => $data['pozice']]);
    }

    public static function updateZam(array $data, $id)
    {
	return DB::table('iis_zamestnanec')
		->where('zamID', $id)
		->update([
            'pracoviste' => $data['pracoviste'],
            'prava' => $data['pozice'],
            'jmeno' => $data['jmeno'],
            'prijmeni' => $data['prijmeni'],
            'datum_narozeni' => $data['datum_narozeni'],
            'mobil' => $data['mobil'],
            'telefon_kancelar' => $data['telefon_kancelar'],
            'mail' => $data['mail'],
        ]);
    }
    public static function deleteZam($id){
	DB::table('iis_zamestnanec')
	    ->where('zamID', $id)
	    ->update(['smazan'=> 1, 'aktivni' => 0,]);
	User::destroy($id);
    }
}
