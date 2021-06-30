cl=3.1622e18
trap_per_free=1e3
N=3.1622e22
time_scaling=1e12
epsilon=10000
length_scaling=1e15

[GlobalParams]
  order = CONSTANT
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 20
  xmax = 1
[]

# [Problem]
#   type = ReferenceResidualProblem
#   extra_tag_vectors = 'ref'
#   reference_vector = 'ref'
# []

[Variables]
  [mobile]
    order = FIRST
  []
  [trapped]
    family = MONOMIAL
  []
[]

[AuxVariables]
  [empty_sites]
    family = MONOMIAL
  []
  [scaled_empty_sites]
    family = MONOMIAL
  []
  [trapped_sites]
    family = MONOMIAL
  []
[]

[AuxKernels]
  [empty_sites]
    variable = empty_sites
    type = EmptySitesAux
    N = ${fparse N / cl}
    Ct0 = .1
    trap_per_free = ${trap_per_free}
    trapped_concentration_variables = trapped
  []
  [scaled_empty]
    variable = scaled_empty_sites
    type = NormalizationAux
    normal_factor = ${cl}
    source_variable = empty_sites
  []
  [trapped_sites]
    variable = trapped_sites
    type = NormalizationAux
    normal_factor = ${trap_per_free}
    source_variable = trapped
  []
[]

[Kernels]
  [./diff]
    type = MatDiffusion
    variable = mobile
    diffusivity = ${fparse length_scaling / time_scaling}
    # extra_vector_tags = ref
  [../]
  [./time]
    type = TimeDerivative
    variable = mobile
    # extra_vector_tags = ref
  [../]
  [coupled_time]
    type = ScaledCoupledTimeDerivative
    variable = mobile
    v = trapped
    factor = ${trap_per_free}
    # extra_vector_tags = ref
  []

  [trapped_time]
    type = TimeDerivative
    variable = trapped
  []
  [trapping]
    type = TrappingKernel
    variable = trapped
    alpha_t = ${fparse 1e15 / time_scaling}
    N = ${fparse 3.1622e22 / cl}
    Ct0 = 0.1
    mobile = 'mobile'
    trap_per_free = ${trap_per_free}
    # extra_vector_tags = ref
  []
  [release]
    type = ReleasingKernel
    alpha_r = ${fparse 1e13 / time_scaling}
    temp = 1000
    trapping_energy = ${epsilon}
    variable = trapped
  []
[]

[BCs]
  [left]
    type = DirichletBC
    variable = mobile
    value = ${fparse 3.1622e18 / cl}
    boundary = left
  []
  [right]
    type = DirichletBC
    variable = mobile
    value = 0
    boundary = right
  []
[]

[Postprocessors]
  [outflux]
    type = SideDiffusiveFluxAverage
    boundary = 'right'
    diffusivity = 1
    variable = mobile
  []
  [scaled_outflux]
    type = ScalePostprocessor
    value = outflux
    scaling_factor = ${cl}
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  end_time = 1000
  dt = 1
  solve_type = NEWTON
  line_search = 'bt'
  automatic_scaling = true
  verbose = true
  compute_scaling_once = false
  abort_on_solve_fail = true
  petsc_options_iname = '-pc_type -snes_max_it -ksp_max_it'
  petsc_options_value = 'lu       10           100'
[]

[Outputs]
  exodus = true
  csv = true
  [dof]
    type = DOFMap
    execute_on = initial
  []
  perf_graph = true
[]
