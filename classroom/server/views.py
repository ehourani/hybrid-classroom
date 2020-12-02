from django.shortcuts import render
from django.http import HttpResponse, HttpResponseNotFound

from states.models import HybridClassroom

# Create your views here.


def home(*args, **kwargs):
    return HttpResponse("6.810 Hybrid Classroom!")


'''Get most recent session's information'''
def session_info(*args, **kwargs):
    return HttpResponse(str(get_most_recent()))


'''Delete all sessions'''
def reset(*args, **kwargs):
    HybridClassroom.objects.all().delete()
    return HttpResponse("Deleted all sessions")


'''
Creates a new session if most recent current session is complete
'''
def session(request, *args, **kwargs):

    # TODO: Take out incompleteness check

    response_string = "Incomplete session with ID "
    
    # First check if the database is empty (add a new session if so)
    if get_most_recent() is None:
        new_session = ServerStates()
        new_session.presentation_mode = True if is_true(request.GET.get('presentation')) else False 
        new_session.save()
        response_string = "Made a new session with ID "

    completed, old_id = get_most_recent().finished_test, get_most_recent().session_id

    # If most recent entry is completed, then make a new session
    if completed:
        new_session = ServerStates()
        new_session.presentation_mode = True if is_true(request.GET.get('presentation')) else False 
        new_session.save()
        return HttpResponse("Made a new session with ID " + str(new_session.session_id))

    return HttpResponse(response_string + str(old_id))


def bear(*args, **kwargs):
    # TODO: implement
    pass


def high_five(request, *args, **kwargs):
    # TODO: implement
    pass


def mousepad(*args, **kwargs):
    # TODO: implement
    pass


def color(request, *args, **kwargs):
    # TODO: implement
    pass


def raise_hand(*args, **kwargs):
    # TODO: implement
    pass


def understand(request, *args, **kwargs):
    # TODO: implement
    pass


def applause(*args, **kwargs):
    # TODO: implement
    pass


'''Helper function to get most recent session'''
def get_most_recent():
    if HybridClassroom.objects.count() > 0:
        return HybridClassroom.objects.all().order_by('-session_id')[0]

    return None