@extends('layouts.app')

@section('content')

<div class="container">
	<div class="panel panel-default">
	    @if ($neprovedene)
	    <div class="panel-heading">
		Neschválené přesuny
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')
		<form action="/presuny/activate" method="POST">
		    {!! csrf_field() !!}
		<div class="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th></th>
			<th>#</th>
			<th>Zařízení</th>
			<th>Odkud</th>
			<th>Kam</th>
			<th>Důvod</th>
			<th>Žadatel</th>
		    </thead>
		    <tbody>
		    @foreach ($neprovedene as $row)
		    <tr>
			<td class="table-text">
			    <input type="checkbox" class="checkbox" name="act[]" value={{ $row->presunID }} />
			</td>
			<td class="table-text">
			    {{ $row->presunID }}
			</td>
			<td class="table-text">
			    #{{ $row->zarID }} {{ $row->znacka }} {{ $row->model }}
			</td>
			<td class="table-text">
			    {{ $row->odkud }}
			</td>
			<td class="table-text">
			    {{ $row->kam }}
			    <input type="hidden" name="kam[]" value={{ $row->kam }} /> 
			</td>
			<td class="table-text">
			    {{ $row->duvod }}
			</td>
			<td class="table-text">
			    {{ $row->prijmeni }} {{ $row->jmeno }}
			</td>
    
		    </tr>
		    @endforeach

		    </tbody>
		</table>
		    <div class="form-group">
			<div class="col-sm-offset-0 col-sm-6">
			    <button type="submit" class="btn btn-default">
				<i class="fa fa-check"></i> Schválit</button>
			</div>
		    </div>
		</form>
		</div>
	    </div>
	@else
	    <div class="panel-heading">
	    Žádné neschválené poruchy.
	    </div>
	@endif
	</div>
</div>

<div class="container">
	<div class="panel panel-default">
	    @if ($provedene)
	    <div class="panel-heading">
		Schválené přesuny
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')
		<div class="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th>Provedeno</th>
			<th>#</th>
			<th>Zařízení</th>
			<th>Odkud</th>
			<th>Kam</th>
			<th>Důvod</th>
			<th>Žadatel</th>
		    </thead>
		    <tbody>
		    @foreach ($provedene as $row)
		    <tr>
			<td class="table-text">
			    {{ $row->provedeno }}
			</td>
			<td class="table-text">
			    {{ $row->presunID }}
			</td>
			<td class="table-text">
			    #{{ $row->zarID }} {{ $row->znacka }} {{ $row->model }}
			</td>
			<td class="table-text">
			    {{ $row->odkud }}
			</td>
			<td class="table-text">
			    {{ $row->kam }}
			</td>
			<td class="table-text">
			    {{ $row->duvod }}
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
	    Žádné schválené přesuny.
	    </div>
	@endif
	</div>
</div>

    <!-- TODO: Current employees -->
@endsection
