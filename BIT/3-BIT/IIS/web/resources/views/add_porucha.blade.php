<!-- resources/views/add_user.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default">
	    <div class="panel-heading">
		Oznámení poruchy zařízení
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

    <form method="POST" action="/porucha/add" class="form-horizontal">
	{!! csrf_field() !!}
<div class="form-group">
	<label for="k_oprave" class="col-sm-5 control-label">
	Zařízení*
	</label>
	<div class="col-sm-7">	
	    <select name="k_oprave" id="k_oprave" class="form-control">
		<option value="">--vyberte hodnotu--</option>
	@foreach ($zarizeni as $row)
		<option value="{{ $row->zarID }}" {{ old('k_oprave') == 
		    $row->zarID ? "selected":"" }} >
		#{{ $row->zarID }} {{ $row->znacka }} {{ $row->model }}
		</option>   
	@endforeach	
	</select>
	</div>
	</div>
<div class="form-group">
	<label for="popis" class="col-sm-5 control-label">
	Popis poruchy
	</label>
	<div class="col-sm-7">	
	<textarea name="popis" id="popis" class="form-control"
	    placeholder="Popis poruchy">{{ old('popis') }}</textarea>
	</div>
	</div>
	

<div class="col-sm-offset-5 col-sm-7">
	* povinné údaje
</div>
<br><br>
<div class=" col-sm-offset-5 col-sm-7">
	<button type="submit" class="btn btn-default">
	<i class="fa fa-plus"></i>Odeslat</button>
</div>
    </form>

	    </div>
	</div>
    </div>
</div>
@endsection
