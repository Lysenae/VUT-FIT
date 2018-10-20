@extends('layouts.app')

@section('content')

<div class="container">
	<div class="panel panel-default">
	    @if ($neopravene)
	    <div class="panel-heading">
		Neopravené poruchy
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')
		<form action="/poruchy/activate" method="POST">
		    {!! csrf_field() !!}
		<div class="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th></th>
			<th>#</th>
			<th>Zařízení</th>
			<th>Oznamující</th>
			<th>Popis vady</th>
		    </thead>
		    <tbody>
		    @foreach ($neopravene as $row)
		    <tr>
			<td class="table-text">
			    <input type="checkbox" class="checkbox" name="act[]" value={{ $row->poruchaID }} />
			</td>
			<td class="table-text">
			    {{ $row->poruchaID }}
			</td>
			<td class="table-text">
			    #{{ $row->zarID }} {{ $row->znacka }} {{ $row->model }}
			</td>
			<td class="table-text">
			    {{ $row->prijmeni }} {{ $row->jmeno }}
			</td>
			<td class="table-text">
			    {{ $row->popis }}
			</td>
    
		    </tr>
		    @endforeach

		    </tbody>
		</table>
		    <div class="form-group">
			<div class="col-sm-offset-0 col-sm-6">
			    <button type="submit" class="btn btn-default">
				<i class="fa fa-check"></i> Opraveno</button>
			</div>
		    </div>
		</form>
		</div>
	    </div>
	@else
	    <div class="panel-heading">
	    Žádné neopravené poruchy.
	    </div>
	@endif
	</div>
</div>

<div class="container">
    <div class="">
	<div class="panel panel-default">
	    @if ($opravene)
	    <div class="panel-heading">
		Opravené poruchy
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')
		<div class="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th>#</th>
			<th>Zařízení</th>
			<th>Technik</th>
		    </thead>
		    <tbody>
		    @foreach ($opravene as $row)
		    <tr>
			<td class="table-text">
			    {{ $row->poruchaID }}
			</td>
			<td class="table-text">
			    #{{ $row->zarID }} {{ $row->znacka }} {{ $row->model }}
			</td>
			<td class="table-text">
			    {{ $row->prijmeni }} {{ $row->jmeno }}
			</td>
		    </tr>
		    @endforeach

		    </tbody>
		</table>
		</div>
	    </div>
	@else
	    <div class="panel-heading">
	    Žádné opravené poruchy.
	    </div>
	@endif
	</div>
</div>

    <!-- TODO: Current employees -->
@endsection
