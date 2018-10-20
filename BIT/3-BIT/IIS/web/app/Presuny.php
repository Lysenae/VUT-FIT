<?php

namespace App;

use Illuminate\Database\Eloquent\Model;
use Validator;
use DB;
use Auth;
class Presuny extends Model
{
    protected function validator(array $data)
    {
	    
	$rules = [
		'zarizeni' => 'required|max:7',
		'duvod' => 'max:255',
		'kam' => 'required|max:7',
	    ];
	return Validator::make($data, $rules);
    }

    public static function createPre(array $data)
    {
	$odkud = DB::select("SELECT umisteno
				FROM iis_zarizeni
				WHERE zarID = $data[zarizeni]");
        DB::table('iis_presun')->insert([
            'zarizeni' => $data['zarizeni'],
            'zadatel' => Auth::user()->id,
            'duvod' => $data['duvod'],
            'odkud' => $odkud[0]->umisteno,
	    'kam' => $data['kam'],
        ]);
    }


    public static function activate(array $data)
    {
	if (isset($data['act']))
	{
	    $datum = date('Y-m-d', time());
	    print_r($data);	
	    foreach($data['act'] as $id => $presun)
	    {
		DB::table('iis_zarizeni')
		    ->where('zarID', $presun)
		    ->update([
			'umisteno' => $data['kam'][$id],
			]);
		DB::table('iis_presun')
		    ->where('presunID', $presun)
		    ->update(['schvalil' => Auth::user()->id]);
	    }
	}
	else 
	   return redirect()->back()->withError(['submit' => 'Musite vybrat nektery zaznam']);
    }
}
