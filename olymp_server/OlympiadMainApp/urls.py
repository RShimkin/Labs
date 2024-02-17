from django.urls import path, re_path

from .views import *

urlpatterns = [
    path('', code, name='home'),
    path('tasks/', tasks, name='tasks'),
    path('edit_stask/<task_oid>/', edit_stask, name='editstask'),
    path('view_stask/<task_oid>/', view_stask, name='viewstask'),
    path('createstask/', create_stask, name='createstask'),
    path('stask/<task_oid>/', stask, name='stask'),
    path('updatetask/<task_oid>/', update_task, name='updatetask'),

    path('olympiads/', olympiads, name='olympiads'),
    path('createolympiad', create_olympiad, name='createolympiad'),
    path('olympiad/<olymp_oid>/', olympiad, name='olympiad'),
    path('view_olympiad/<olymp_oid>/', view_olympiad, name='viewolympiad'),
    path('edit_olympiad/<olymp_oid>/', edit_olympiad, name='editolympiad'),

    path('add_itask/<olymp_oid>/', add_itask, name='additask'),
    path('edit_itask/<task_oid>/', edit_itask, name='edititask'),
    #path('updatetask/<task_name>/', update_task, name='updatetask'),
    path('test/', test, name='test'),
    path('filetest/', filetest, name='filetest'),
    path('servertime/', servertime, name='servertime')
]