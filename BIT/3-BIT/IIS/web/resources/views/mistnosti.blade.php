@extends('layouts.app')

@section('content')

<div class="container">
    <div class="panel panel-default">
	<div class="panel-heading">
		Seznam kanceláří
	</div>

	<div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

	    <div class="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th>#</th>
			<th>Oddělení</th>
			<th>Lokalita</th>
			<th>Rozměry[m]</th>
			<th>El. zás.</th>
			<th>Eth. zás.</th>
			<th> </th>
		    </thead>
		    <tbody>
		    @foreach ($offices as $row)
		    <tr>
			<td class="table-text">
			    {{ $row->roomID }}
			</td>
			<td class="table-text">
			    {{ $row->nazev }}
			</td>
			<td class="table-text">
			    {{ $row->lokace }}
			</td>
			<td class="table-text">
			    {{ $row->sirka }} x {{ $row->delka }} x {{ $row->vyska }}
			</td>
			<td class="table-text">
			    {{ $row->el_zasuvky }}
			</td>
			<td class="table-text">
			    {{ $row->eth_zasuvky }}
			</td>
			<td class="table-text">
			    <a href="/kancelar/{{ $row->roomID }}/show">upravit</a>
			</td>
		    </tr>
		    @endforeach
		    </tbody>
		</table>
	    </div>		
	    <form action="/kancelar" method="POST" class="form-horizontal">
		{{ csrf_field() }}

		<!-- Add employee Button -->
		<div class="form-group">
		    <div class="col-sm-offset-0 col-sm-6">
			<button type="submit" class="btn btn-default">
			    <i class="fa fa-plus"></i> Přidat kancelář</button>
		    </div>
		</div>
	    </form>
	</div>
    </div>
</div>
<div class="container">
    <div class="panel panel-default">
        <div class="panel-heading">
	    Seznam učeben
	</div>

        <div class="panel-body">
        <!-- Display Validation Errors -->
	    @include('common.errors')
	    <div class ="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th>#</th>
			<th>Oddělení</th>
			<th>Lokalita</th>
			<th>Rozměry[m]</th>
			<th>Kapacita</th>
			<th>Řad</th>
			<th>Bloků</th>
			<th> </th>
		    </thead>
		    <tbody>
		    @foreach ($classes as $row)
			<tr>
			    <td class="table-text">
				{{ $row->roomID }}
			    </td>
			    <td class="table-text">
				{{ $row->nazev }}
			    </td>
			    <td class="table-text">
				{{ $row->lokace }}
			    </td>
			    <td class="table-text">
				{{ $row->sirka }} x {{ $row->delka }} x {{ $row->vyska }}
			    </td>
			    <td class="table-text">
				{{ $row->mist_k_sezeni}}
			    </td>
			    <td class="table-text">
				{{ $row->pocet_rad }}
			    </td>
			    <td class="table-text">
				{{ $row->bloky }}
			    </td>
			    <td class="table-text">
				<a href="/ucebna/{{ $row->roomID }}/show">upravit</a>
			    </td>
			</tr>
		    @endforeach
		    </tbody>
		</table>
	    </div>
	    <form action="/ucebna" method="POST" class="form-horizontal">
		{{ csrf_field() }}

		<!-- Add employee Button -->
		<div class="form-group">
		    <div class="col-sm-offset-0 col-sm-6">
			<button type="submit" class="btn btn-default">
			    <i class="fa fa-plus"></i> Přidat učebnu</button>
		    </div>
		</div>
	    </form>
	</div>
    </div>
</div>
@endsection
