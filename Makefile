###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
# MOOSE_DIR        - Root directory of the MOOSE project
#
###############################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
MOOSE_SUBMODULE    := $(CURDIR)/moose
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR        ?= $(shell dirname `pwd`)/moose
endif

THM_SUBMODULE    := $(CURDIR)/thm
ifneq ($(wildcard $(THM_SUBMODULE)/Makefile),)
  THM_DIR        ?= $(THM_SUBMODULE)
else
  THM_DIR        ?= $(shell dirname `pwd`)/thm
endif
# check that THM is available
THM_CONTENT := $(shell ls $(THM_DIR) 2> /dev/null)
ifeq ($(THM_CONTENT),)
  $(error THM does not seem to be available. Make sure that either the submodule is checked out or that your THM_DIR points to the correct location)
endif

# framework
FRAMEWORK_DIR      := $(MOOSE_DIR)/framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

################################## MODULES ####################################
# To use certain physics included with MOOSE, set variables below to
# yes as needed.  Or set ALL_MODULES to yes to turn on everything (overrides
# other set variables).

ALL_MODULES                 := no

CHEMICAL_REACTIONS          := no
CONTACT                     := no
EXTERNAL_PETSC_SOLVER       := no
FLUID_PROPERTIES            := yes
FUNCTIONAL_EXPANSION_TOOLS  := no
HEAT_CONDUCTION             := yes
LEVEL_SET                   := no
MISC                        := yes
NAVIER_STOKES               := yes
PHASE_FIELD                 := no
POROUS_FLOW                 := no
RAY_TRACING                 := yes
RDG                         := yes
RICHARDS                    := no
SOLID_MECHANICS             := no
STOCHASTIC_TOOLS            := no
TENSOR_MECHANICS            := no
XFEM                        := no

include $(MOOSE_DIR)/modules/modules.mk
###############################################################################

# THM
APPLICATION_DIR    := $(THM_DIR)
APPLICATION_NAME   := thm
include            $(FRAMEWORK_DIR)/app.mk

# dep apps
APPLICATION_DIR    := $(CURDIR)
APPLICATION_NAME   := tmap
BUILD_EXEC         := yes
GEN_REVISION       := no
include            $(FRAMEWORK_DIR)/app.mk

###############################################################################
# Additional special case targets should be added here
